# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0026_ble_tag_timestamp'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='ble_tag',
            name='timestamp',
        ),
    ]
