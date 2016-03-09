# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import django.utils.timezone


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0025_auto_20150810_1507'),
    ]

    operations = [
        migrations.AddField(
            model_name='ble_tag',
            name='timestamp',
            field=models.DateTimeField(default=django.utils.timezone.now, blank=True),
        ),
    ]
