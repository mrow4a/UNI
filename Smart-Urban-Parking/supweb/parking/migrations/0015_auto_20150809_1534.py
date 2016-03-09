# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import django.utils.timezone


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0014_auto_20150805_1301'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='lamp',
            name='online_timestamp',
        ),
        migrations.AlterField(
            model_name='ble_tag',
            name='timestamp',
            field=models.DateTimeField(default=django.utils.timezone.now, blank=True),
        ),
    ]
